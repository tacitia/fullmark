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

$instDate = $_POST["instdate"];
$district = $_POST["on_after"];

$conn = ocilogon("comp231stu109", "xiaosa19890213", "comp231.cse.ust.hk");

if (!$conn) {
	echo "<h1>Unable to connect </h1>\n";
	die();
}

$curs = ocinewcursor($conn);
if ($district == "All") {
       	$parse = ociparse($conn, "begin FindAvailEng(:Date, :Result); end;");
	

} else if ($district == "NT") {
	$parse = ociparse($conn, "begin FindAvailEngD(:Date, :District, :Result); end;");
	ocibindbyname($parse, "District", $district);
} else if ($district == "KLN") {
	$parse = ociparse($conn, "begin FindAvailEngD(:Date, :District, :Result); end;");
	ocibindbyname($parse, "District", $district);
} else {
	$parse = ociparse($conn, "begin FindAvailEngD(:Date, :District, :Result); end;");
	ocibindbyname($parse, "District", $district);
}
ocibindbyname($parse, "Date", $instDate);
ocibindbyname($parse, "Result", $curs, -1, OCI_B_CURSOR);ociexecute($parse);
ociexecute($curs);

if ($district == "All") {
	echo "<table border=\"1\" align=\"center\">";
	echo "<tr><th>Eng. ID</th><th>Eng. Name</th><th>Resp. District</th><th># of Installation Task</th></tr>";

	while ($row = oci_fetch_row($curs)) {
		echo "<tr><td>$row[0]</td><td>$row[1]</td><td>$row[2]</td><td>$row[3]</td></tr>\n";
	}
} else {
	echo "<table border=\"1\" align=\"center\">";
	echo "<tr><th>Eng. ID</th><th>Eng. Name</th><th># of Installation Tasks</th></tr>";
	while ($row = oci_fetch_row($curs)) {
		echo  "<tr><td>$row[0]</td><td>$row[1]</td><td>$row[2]</td></tr>\n";
	}
}
oci_close($conn);
?>

</center></body>
</html>
