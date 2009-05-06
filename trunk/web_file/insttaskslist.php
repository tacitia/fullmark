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

$engID = $_POST["eid"];
$mode = $_POST["on_after"];
$date = $_POST["instdate"];

$conn = ocilogon("comp231stu109", "xiaosa19890213", "comp231.cse.ust.hk");

if (!$conn) {
	echo "<h1>Unable to connect </h1>\n";
	die();
}
$curs = ocinewcursor($conn);
if ($mode == "On") {
	$parse = ociparse($conn, "begin EngTaskList(:Eid, :Date, :Result); end;");
} else {
	$parse = ociparse($conn, "begin EngTaskListA(:Eid, :Date, :Result); end;");
}
ocibindbyname($parse, "Eid", $engID);
ocibindbyname($parse, "Date", $date);
ocibindbyname($parse, "Result", $curs, -1, OCI_B_CURSOR);ociexecute($parse);
ociexecute($curs); 

echo "<table border=\"1\" align=\"center\">";
echo "<tr><th>Sub. ID</th><th>Customer Name</th><th>Installation Address</th><th>Phone</th></tr>";
while ($row = oci_fetch_row($curs)) {
	echo "<tr><td>$row[0]</td><td>$row[1]</td><td>$row[2]</td><td>$row[3]</td></tr>\n";
}
oci_close($conn);
?>

</center></body>
</html>
