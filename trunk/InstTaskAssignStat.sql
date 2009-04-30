CREATE OR REPLACE PROCEDURE InstTaskAssignStat 
	(SDate IN char, EDate IN char, Result OUT sys_refcursor) AS
startDate DATE;
endDate DATE;
BEGIN
	startDate := to_date(SDate, 'yymmdd');
	endDate := to_date(EDate,  'yymmdd');
	OPEN Result FOR
		SELECT e.Eng_ID, e.Name, COUNT(i.Task_ID)
			FROM Engineers e
			LEFT OUTER JOIN Installations i
			ON e.Eng_ID = i.Eng_ID
			WHERE (i.Install_date >= startDate AND i.Install_date <= endDate) OR i.Install_date IS NULL
			GROUP BY e.Eng_ID, e.Name;
END;
/