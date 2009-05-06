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

$subID = $_POST["sid"];
$engID = $_POST["eid"];
$function = $_POST["mode"];

$conn = ocilogon("comp231stu109", "xiaosa19890213", "comp231.cse.ust.hk");

if (!$conn) {
	echo "<h1>Unable to connect </h1>\n";
	die();
}

if($function == "auto") {
	$notAssigned = 0;
	$parse = ociparse($conn, "begin AssignInstTaskA(:Result); end;");
	ocibindbyname($parse, "Result", $notAssigned);
	ociexecute($parse);
	if ($notAssigned == 0) {
		echo "<h2>Assignment is completed successfully!</h2>\n";
	} else {
		echo "<h2>$notAssigned subscriptions have no installation assigned!</h2>\n";
	}
	
} else {
	$errCode = 0;
	$parse = ociparse($conn, "begin AssignInstTask(:Sid, :Eid , :Errcode); end;");
	ocibindbyname($parse, "Sid", $subID);
	ocibindbyname($parse, "Eid", $engID);
	ocibindbyname($parse, "Errcode", $errCode);
	ociexecute($parse);
	if ($errCode == 0) {
		echo "<h2>Assignment is completed successfully!<\h2>\n";
	} else {
		echo "<h2>There is a problem on the assignment!<\h2>\n";
	}
}
?> 

</center></body>
</html>
