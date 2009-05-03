CREATE OR REPLACE PROCEDURE NewSublist
	(Result OUT sys_refcursor) AS
BEGIN
	OPEN Result FOR
		SELECT hkid, name, contact_phone_num, installation_address, district
		FROM Subscriptions NATURAL JOIN Customers
		WHERE Subscriptions.sub_id
		NOT IN 
		(SELECT sub_id FROM Installations);
End;
/


CREATE OR REPLACE PROCEDURE EngTaskList
	(EID IN number, InstDate IN char, Result OUT sys_refcursor) AS
	InDate date;
BEGIN
	InDate := TO_DATE(InstDate, 'YYMMDD');
	OPEN Result FOR
		SELECT sub_id, name, installation_address, contact_phone_num
		FROM Customers NATURAL JOIN Subscriptions NATURAL JOIN Installations
		WHERE eng_id = EID AND Install_date = InDate;
End;
/


CREATE OR REPLACE PROCEDURE EngTaskListA
	(EID IN number, InstDate IN char, Result OUT sys_refcursor) AS
	InDate date;
BEGIN
	InDate := TO_DATE(InstDate, 'YYMMDD');
	OPEN Result FOR
		SELECT sub_id, name, installation_address, contact_phone_num
		FROM Customers NATURAL JOIN Subscriptions NATURAL JOIN Installations
		WHERE eng_id = EID AND Install_date > InDate;
End;
/

CREATE OR REPLACE PROCEDURE FindAvailEng
	(InstDate IN char, Result OUT sys_refcursor) AS
	InDate date;
BEGIN
	InDate := TO_DATE(InstDate, 'YYMMDD');
	OPEN Result FOR
		SELECT eng_id, name, resp_district, count(*)
		FROM Installations NATURAL JOIN Engineers
		WHERE Install_date = InDate
		GROUP BY eng_id, name, resp_district
		HAVING count(*) < 4;
End;
/


CREATE OR REPLACE PROCEDURE FindAvailEngD
	(InstDate IN char, District IN char, Result OUT sys_refcursor) AS
	InDate date;
BEGIN
	InDate := TO_DATE(InstDate, 'YYMMDD');
	OPEN Result FOR
		SELECT eng_id, name, count(*)
		FROM Installations NATURAL JOIN Engineers
		WHERE Install_date = InDate AND resp_district = District
		GROUP BY eng_id, name
		HAVING count(*) < 4;
End;
/

CREATE OR REPLACE PROCEDURE AssginInstTask
	(SID IN number, EID IN number, ErrCode OUT number) AS
	isInstalled	BOOLEAN;
	engDistrict	Engineers.Resp_district%type;
	subDistrict	Subscriptions.District%type;
	instDate	Subscriptions.Prefer_install_date%type;
	workLoad	NUMBER;
	minLoad	NUMBER;
	tempLoad	NUMBER;
	taskID	Installations.Task_ID%type;
BEGIN
	ErrCode := 0;
	isInstalled := FALSE;
	minLoad := 4;
	
	/*initialize engDistrict*/
	SELECT Resp_district INTO engDistrict
		FROM Engineers
		WHERE Eng_ID = EID;
	
	/*initialize subDistrict and instDate*/
	SELECT District, Prefer_install_date 
		INTO subDistrict, instDate
		FROM Subscriptions
		WHERE Sub_ID = SID;
	
	/*initialize taskID*/
	SELECT MAX(Task_ID) + 1 INTO taskID
		FROM Installations;
	
	/*check whether it's installed*/
	FOR R IN(
		SELECT * FROM Installations
			WHERE Sub_ID = SID
	)LOOP
		isInstalled := TRUE;
	END LOOP;
	
	IF NOT isInstalled THEN
		SELECT DISTINCT COUNT(*) INTO workLoad
			FROM Installations
			WHERE Eng_ID = EID AND Install_date = instDate;
		/* check workload is less than 4*/
		IF workLoad < 4 THEN

			FOR R IN(
				SELECT DISTINCT Eng_ID
					FROM Engineers
					WHERE Resp_district = subDistrict
			) LOOP
				SELECT DISTINCT COUNT(*) INTO tempLoad
					FROM Subscriptions NATURAL JOIN Installations NATURAL JOIN Engineers
					WHERE Eng_ID = R.Eng_ID AND Prefer_install_date = instDate;
				IF tempLoad < minLoad THEN
				minLoad := tempLoad;
				END IF;
			END LOOP;		
			IF minLoad >= workLoad THEN
				/*check the district*/
				IF subDistrict = engDistrict THEN
					INSERT INTO Installations
						VALUES(taskID, SID, EID, instDate);
				ELSE
					IF (minLoad >= 4 
						AND ((engDistrict = 'HK' AND subDistrict = 'KLN')
							OR (engDistrict = 'KLN' AND (subDistrict = 'HK' OR subDistrict = 'NT'))
							OR (engDistrict = 'NT' AND subDistrict = 'KLN'))) THEN
										FOR R IN(
												SELECT DISTINCT ENG_ID
													FROM Engineers
													WHERE Resp_district = engDistrict
												) LOOP
													SELECT DISTINCT COUNT(*) INTO tempLoad
													FROM Subscriptions NATURAL JOIN Installations NATURAL JOIN Engineers
													WHERE Eng_ID = R.ENG_ID AND Prefer_install_date = instDate;
													IF tempLoad < minLoad THEN
													minLoad := tempLoad;
													END IF;
												END LOOP;
										IF minLoad >= workLoad THEN
											INSERT INTO Installations
												VALUES(taskID, SID, EID, instDate);
										ELSE
											ErrCode := 1;
										END IF;
					ELSE
						ErrCode := 1;
					END IF;
				END IF;
			ELSE
				ErrCode := 1;
			END IF;
		ELSE
			ErrCode := 1;
		END IF;
	ELSE
		ErrCode := 1;
	END IF;
EXCEPTION
	WHEN NO_DATA_FOUND THEN
	ErrCode := 1;
END;
/

CREATE OR REPLACE PROCEDURE AssginInstTaskA
	(NotAssigned OUT number) AS
	ErrCode	NUMBER;
	EID	Engineers.Eng_ID%type;
	EName	Engineers.Name%type;
	workLoad	NUMBER;
	charDate	CHAR(30);
	availEngList		SYS_REFCURSOR;
	hkAvailEngList	SYS_REFCURSOR;
	klnAvailEngList	SYS_REFCURSOR;
	ntAvailEngList	SYS_REFCURSOR;
BEGIN
	NotAssigned := 0;
	ErrCode := 1;
	FOR R IN(
		SELECT s.Sub_ID Sub_ID, Prefer_install_date, District
		FROM Subscriptions s
		LEFT OUTER JOIN Installations i
		ON s.Sub_ID = i.Sub_ID
		WHERE i.Sub_ID IS NULL)LOOP
		charDate := to_char(R.Prefer_install_date, 'yymmdd');
		FindAvailEngD(R.Prefer_install_date, R.District, availEngList);
		LOOP
			FETCH availEngList INTO EID, EName, workLoad;
			EXIT WHEN availEngList%NOTFOUND;
			AssginInstTask(R.Sub_ID, EID, ErrCode);
			EXIT WHEN ErrCode = 0;
		END LOOP;
		CLOSE availEngList;
		IF ErrCode = 1 THEN
			IF R.District = 'HK' OR R.District = 'NT' THEN
				FindAvailEngD(charDate, 'KLN', klnAvailEngList);
				LOOP
					FETCH klnAvailEngList INTO EID, EName, workLoad;
					EXIT WHEN klnAvailEngList%NOTFOUND;
					AssginInstTask(R.Sub_ID, EID, ErrCode);
					EXIT WHEN ErrCode = 0;
				END LOOP;
				CLOSE klnAvailEngList;
			END IF;
			IF R.District = 'KLN' THEN
				FindAvailEngD(charDate, 'HK', hkAvailEngList);
				LOOP
					FETCH hkAvailEngList INTO EID, EName, workLoad;
					EXIT WHEN klnAvailEngList%NOTFOUND;
					AssginInstTask(R.Sub_ID, EID, ErrCode);
					EXIT WHEN ErrCode = 0;
				END LOOP;
				CLOSE hkAvailEngList;
				IF ErrCode = 1 THEN
					FindAvailEngD(charDate, 'NT', ntAvailEngList);
					LOOP
						FETCH ntAvailEngList INTO EID, EName, workLoad;
						EXIT WHEN ntAvailEngList%NOTFOUND;
						AssginInstTask(R.Sub_ID, EID, ErrCode);
						EXIT WHEN ErrCode = 0;
					END LOOP;
					CLOSE ntAvailEngList;
				END IF;
			END IF;
		END IF;

		IF ErrCode = 1 THEN
			NotAssigned := NotAssigned + 1;
		END IF; 
	END LOOP;
	
END;
/

CREATE OR REPLACE PROCEDURE DelAssignInstTask
	(TID IN number) AS	
BEGIN
	DELETE FROM Installations
		WHERE Task_ID = TID;
END;
/

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

CREATE OR REPLACE PROCEDURE InstTaskDistriStat 
	(SDate IN char, EDate IN char, Result OUT sys_refcursor) AS
startDate	DATE;
endDate	DATE;
BEGIN
	startDate := to_date(SDate, 'yymmdd');
	endDate := to_date(EDate, 'yymmdd');
	OPEN Result FOR
		SELECT 'HK', (SELECT COUNT(i.Task_ID)
			FROM Subscriptions s
			LEFT OUTER JOIN Installations i
			ON s.Sub_ID = i.Sub_ID
			WHERE s.District = 'HK' AND ((i.Install_date >= startDate AND i.Install_date <= endDate) OR i.Install_date IS NULL)) FROM DUAL
		UNION ALL
		SELECT 'KLN', (SELECT COUNT(i.Task_ID)
			FROM Subscriptions s
			LEFT OUTER JOIN Installations i
			ON s.Sub_ID = i.Sub_ID
			WHERE s.District = 'KLN' AND ((i.Install_date >= startDate AND i.Install_date <= endDate) OR i.Install_date IS NULL)) FROM DUAL
		UNION ALL
		SELECT 'NT', (SELECT COUNT(i.Task_ID)
			FROM Subscriptions s
			LEFT OUTER JOIN Installations i
			ON s.Sub_ID = i.Sub_ID
			WHERE s.District = 'NT' AND ((i.Install_date >= startDate AND i.Install_date <= endDate) OR i.Install_date IS NULL)) FROM DUAL;
END;
/
