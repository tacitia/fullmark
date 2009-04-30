CREATE OR REPLACE PROCEDURE InstTaskDistriStat 
	(SDate IN char, EDate IN char, Result OUT sys_refcursor) AS
startDate	DATE;
endDate	DATE;
BEGIN
	startDate := to_date(SDate, 'yymmdd');
	endDate := to_date(EDate, 'yymmdd');
	OPEN Result FOR
		SELECT s.District, COUNT(i.Task_ID)
		FROM Subscriptions s
		LEFT OUTER JOIN Installations i
		ON s.Sub_ID = i.Sub_ID
		WHERE (i.Install_date >= startDate AND i.Install_date <= endDate) OR i.Install_date IS NULL
		GROUP BY s.District;
END;
/