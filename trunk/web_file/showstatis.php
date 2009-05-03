<html>
<body><center>

<?
error_reporting(0); # what's the use of this line?
$ORACLE_HOME = "C:\oraclexe\app\oracle\product\10.2.0\server";
$PATH = getenv("PATH") . ":$ORACLE_HOME\bin";
$TNS_ADMIN = "$ORACLE_HOME\NETWORK\ADMIN";

putenv("ORACLE_HOME=$ORACLE_HOME");
putenv("PATH=$PATH");
putenv("TNS_ADMIN=$TNS_ADMIN");

$sDate = $_POST["sdate"];
$eDate = $_POST["edate"];
$mode = $_POST["mode"];

###########################################################
#error here. Do not know how to connect to Oracle database
$conn = ocilogon("hr", "1234", "127.0.0.1");

if (!$conn) {
	echo "<h1>Unable to connect </h1>\n";
	die();
}
###########################################################

if($function == "engineer") {
	$curs = ocinewcursor($conn);
	$parse = ociparse($conn, "begin InstTaskAssignStat($sDate, $eDate, :data); end;");
	ocibindbyname($parse, "data", $curs, -1, OCI_B_CURSOR);
	ociexecute($parse);
	ociexecute($curs);
?> 

<table border="1" align="center">
<tr><th>Engineer ID</th><th>Engineer Name</th><th>Task Number</th></tr>

<?
	while ($row = oci_fetch_row($curs)) {
		echo "<tr><td>$row[0]</td><td>$row[1]</td><td>$row[2]</td></tr>\n";
	}
	oci_close($conn);
} else {
	$curs = ocinewcursor($conn);
	$parse = ociparse($conn, "begin InstTaskDistriStat($sDate, $eDate, :data); end;");
	ocibindbyname($parse, "data", $curs, -1, OCI_B_CURSOR);
	ociexecute($parse);
	ociexecute($curs);
?>

<table border="1" align="center">
<tr><th>District</th><th>Task Number</th></tr>

<?
	while ($row = oci_fetch_row($curs)) {
		echo "<tr><td>$row[0]</td><td>$row[1]</td></tr>\n";
	}
	oci_close($conn);
}
?>

<form method="POST" action="assigntask.html">
	<input type="submit" value="Back" />
</form>
</center></body>
</html>