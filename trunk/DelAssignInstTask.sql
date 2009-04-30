CREATE OR REPLACE PROCEDURE DelAssignInstTask
	(TID IN number) AS	
BEGIN
	DELETE FROM Installations
		WHERE Task_ID = TID;
END;
/