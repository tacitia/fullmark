<html>
<body><center>

<?
error_reporting(0);
$ORACLE_HOME = "/usr/local/dbpackages/oracle";
$PATH = getenv("PATH") . ":$ORACLE_HOME/bin";
$TNS_ADMIN = "$ORACLE_HOME/network/admin";

putenv("ORACLE_HOME=$ORACLE_HOME");
putenv("PATH=$PATH");
putenv("TNS_ADMIN=$TNS_ADMIN");

$sDate = $_POST["sdate"];
$eDate = $_POST["edate"];
$mode = $_POST["mode"];

$conn = ocilogon("comp231stu109", "xiaosa19890213", "comp231.cse.ust.hk");

if (!$conn) {
	echo "<h1>Unable to connect </h1>\n";
	die();
}

if($mode == "engineer") {
	$curs = ocinewcursor($conn);
	$parse = ociparse($conn, "begin InstTaskAssignStat(:Start, :End, :Result); end;");
	ocibindbyname($parse, "Start", $sDate);
	ocibindbyname($parse, "End", $eDate);
	ocibindbyname($parse, "Result", $curs, -1, OCI_B_CURSOR);
	ociexecute($parse);
	ociexecute($curs);


	echo "<table border=\"1\" align=\"center\">";
	echo "<tr><th>Engineer ID</th><th>Engineer Name</th><th>Task Number</th></tr>";

	while ($row = oci_fetch_row($curs)) {
		echo "<tr><td>$row[0]</td><td>$row[1]</td><td>$row[2]</td></tr>\n";
	}
	oci_close($conn);
} else {
	$curs = ocinewcursor($conn);
	$parse = ociparse($conn, "begin InstTaskDistriStat(:Start, :End, :Result); end;");
	ocibindbyname($parse, "Start", $sDate);
	ocibindbyname($parse, "End", $eDate);
	ocibindbyname($parse, "Result", $curs, -1, OCI_B_CURSOR);
	ociexecute($parse);
	ociexecute($curs);

        echo "<table border=\"1\" align=\"center\">";
        echo "<tr><th>District</th><th>Task Number</th></tr>";


	while ($row = oci_fetch_row($curs)) {
		echo "<tr><td>$row[0]</td><td>$row[1]</td></tr>\n";
	}
	oci_close($conn);
}

?>

</center></body>
</html>
