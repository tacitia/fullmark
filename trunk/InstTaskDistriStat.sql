CREATE OR REPLACE PROCEDURE InstTaskDistriStat 
	(SDate IN char, EDate IN char, Result OUT sys_refcursor) AS
startDate	DATE;
endDate	DATE;
BEGIN
	startDate := to_date(SDate, 'yymmdd');
	endDate := to_date(EDate, 'yymmdd');
	OPEN Result FOR
		SELECT 'HK ' || COUNT(i.Task_ID)
		FROM Subscriptions s
		LEFT OUTER JOIN Installations i
		ON s.Sub_ID = i.Sub_ID AND s.District = 'HK'
		WHERE (i.Install_date >= startDate AND i.Install_date <= endDate) OR i.Install_date IS NULL;
		UNION
		SELECT 'KLN' || COUNT(i.Task_ID)
		FROM Subscriptions s
		LEFT OUTER JOIN Installations i
		ON s.Sub_ID = i.Sub_ID AND s.District = 'KLN'
		WHERE (i.Install_date >= startDate AND i.Install_date <= endDate) OR i.Install_date IS NULL;
		UNION
		SELECT 'NT ' || COUNT(i.Task_ID)
		FROM Subscriptions s
		LEFT OUTER JOIN Installations i
		ON s.Sub_ID = i.Sub_ID AND s.District = 'NT'
		WHERE (i.Install_date >= startDate AND i.Install_date <= endDate) OR i.Install_date IS NULL;
END;
/