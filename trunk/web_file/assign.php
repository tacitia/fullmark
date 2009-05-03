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

$subID = $_POST["sid"];
$engID = $_POST["eid"];
$function = $_POST["mode"];
###########################################################
#error here. Do not know how to connect to Oracle database
$conn = ocilogon("hr", "1234", "127.0.0.1");

if (!$conn) {
	echo "<h1>Unable to connect </h1>\n";
	die();
}
###########################################################

if($function == "auto") {
	$notAssigned = 0;
	$parse = ociparse($conn, "begin AssignInstTaskA($notAssigned); end;");
	ociexecute($parse);
	if ($notAssigned == 0) {
		echo "<h2>Assignment is completed successfully!<\h2>\n";
	} else {
		echo "<h2>$notAssigned subscriptions have no installation assigned!<\h2>\n";
	}
	
} else {
	$errCode = 0;
	$parse = ociparse($conn, "begin AssignInstTask($subID, $engID, $errCode); end;");
	ociexecute($parse);
	if ($errCode == 0) {
		echo "<h2>Assignment is completed successfully!<\h2>\n";
	} else {
		echo "<h2>There is a problem on the assignment!<\h2>\n";
	}
}
?> 

<form method="POST" action="assigntask.html">
	<input type="submit" value="Back" />
</form>
</center></body>
</html>