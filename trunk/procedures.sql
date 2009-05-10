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
		SELECT eng_id, name, resp_district, count(task_ID)
		FROM Engineers
		NATURAL JOIN Installations
		WHERE Install_date = InDate
		GROUP BY eng_id, name, resp_district
		HAVING count(task_ID) < 4
		UNION
		SELECT eng_id, name, resp_district, 0
		FROM Engineers e
		WHERE eng_id NOT IN(SELECT eng_id
						FROM Engineers
						NATURAL JOIN Installations
						WHERE Install_date = InDate);
End;
/


CREATE OR REPLACE PROCEDURE FindAvailEngD
	(InstDate IN char, District IN char, Result OUT sys_refcursor) AS
	InDate date;
BEGIN
	InDate := TO_DATE(InstDate, 'YYMMDD');
	OPEN Result FOR
		SELECT eng_id, name, count(task_ID)
		FROM Engineers
		NATURAL JOIN Installations
		WHERE Install_date = InDate AND resp_district = District
		GROUP BY eng_id, name
		HAVING count(task_ID) < 4
		UNION
		SELECT eng_id, name, 0
		FROM Engineers
		WHERE resp_district = District AND eng_id NOT IN(SELECT eng_id
										      FROM Engineers
										      NATURAL JOIN Installations
										      WHERE Install_date = InDate AND resp_district = District);
		
End;
/

CREATE OR REPLACE PROCEDURE AssignInstTask
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
	
	/*initialize engDistrict, set errCode = 1 in exception handling part if record not found*/
	SELECT Resp_district INTO engDistrict
		FROM Engineers
		WHERE Eng_ID = EID;
	
	/*initialize subDistrict and instDate, set errCode = 1 in exception handling part if record not found*/
	SELECT District, Prefer_install_date 
		INTO subDistrict, instDate
		FROM Subscriptions
		WHERE Sub_ID = SID;
	
	/*initialize taskID*/
	SELECT MAX(Task_ID) + 1 INTO taskID
		FROM Installations;
	IF taskID IS NULL THEN
		taskID := 000001;
	END IF;
	
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
			INSERT INTO Installations VALUES(taskID, SID, EID, instDate);
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

CREATE OR REPLACE PROCEDURE AssignInstTaskA
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
	minLoad	NUMBER;
BEGIN
	NotAssigned := 0;
	ErrCode := 1;
	/*assign task in same district*/
	FOR R1 IN(
		SELECT Sub_ID, Prefer_install_date, District
		FROM Subscriptions
		WHERE Sub_ID NOT IN (SELECT Sub_ID FROM Installations))LOOP
		charDate := to_char(R1.Prefer_install_date, 'YYMMDD');
		ErrCode := 1;
		minLoad := 4;
		FindAvailEngD(charDate, R1.District, availEngList);
		LOOP
			FETCH availEngList INTO EID, EName, workLoad;
			EXIT WHEN availEngList%NOTFOUND;
			IF workLoad <= minLoad THEN
				minLoad := workLoad;
			END IF;
		END LOOP;
		CLOSE availEngList;
		FindAvailEngD(charDate, R1.District, availEngList);
		LOOP
			FETCH availEngList INTO EID, EName, workLoad;
			EXIT WHEN availEngList%NOTFOUND;
			IF workLoad = minLoad THEN
				AssignInstTask(R1.Sub_ID, EID, ErrCode);
			END IF;
			EXIT WHEN ErrCode = 0;
		END LOOP;
		CLOSE availEngList;
	END LOOP;
	
	/*assign task in different district*/
	FOR R IN(
		SELECT Sub_ID, Prefer_install_date, District
		FROM Subscriptions
		WHERE Sub_id not in (SELECT Sub_id FROM Installations))LOOP
			charDate := to_char(R.Prefer_install_date, 'YYMMDD');
			ErrCode := 1;
			minLoad := 4;
			IF R.District = 'HK' OR R.District = 'NT' THEN
				FindAvailEngD(charDate, 'KLN', klnAvailEngList);
				LOOP
					FETCH klnAvailEngList INTO EID, EName, workLoad;
					EXIT WHEN klnAvailEngList%NOTFOUND;
					IF workLoad <= minLoad THEN
						minLoad := workLoad;
					END IF;
				END LOOP;	
				CLOSE klnAvailEngList;
				FindAvailEngD(charDate, 'KLN', klnAvailEngList);
				LOOP
					FETCH klnAvailEngList INTO EID, EName, workLoad;
					EXIT WHEN klnAvailEngList%NOTFOUND;
					IF workLoad <= minLoad THEN
						AssignInstTask(R.Sub_ID, EID, ErrCode);
					END IF;
					EXIT WHEN ErrCode = 0;
				END LOOP;
				CLOSE klnAvailEngList;
			END IF;
			IF R.District = 'KLN' THEN
				FindAvailEngD(charDate, 'HK', hkAvailEngList);
				LOOP
					FETCH hkAvailEngList INTO EID, EName, workLoad;
					EXIT WHEN hkAvailEngList%NOTFOUND;
					IF workLoad <= minLoad THEN
						minLoad := workLoad;
					END IF;
				END LOOP;	
				CLOSE hkAvailEngList;
				FindAvailEngD(charDate, 'HK', hkAvailEngList);
				LOOP
					FETCH hkAvailEngList INTO EID, EName, workLoad;
					EXIT WHEN hkAvailEngList%NOTFOUND;
					IF workLoad <= minLoad THEN
						AssignInstTask(R.Sub_ID, EID, ErrCode);
					END IF;
					EXIT WHEN ErrCode = 0;
				END LOOP;
				CLOSE hkAvailEngList;
				IF ErrCode = 1 THEN
					FindAvailEngD(charDate, 'NT', ntAvailEngList);
					LOOP
						FETCH ntAvailEngList INTO EID, EName, workLoad;
						EXIT WHEN ntAvailEngList%NOTFOUND;
						IF workLoad <= minLoad THEN
							minLoad := workLoad;
						END IF;
					END LOOP;	
					CLOSE ntAvailEngList;
					FindAvailEngD(charDate, 'NT', ntAvailEngList);
					LOOP
						FETCH ntAvailEngList INTO EID, EName, workLoad;
						EXIT WHEN ntAvailEngList%NOTFOUND;
						IF workLoad <= minLoad THEN
							AssignInstTask(R.Sub_ID, EID, ErrCode);
						END IF;
						EXIT WHEN ErrCode = 0;
					END LOOP;
					CLOSE ntAvailEngList;
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
	startDate := to_date(SDate, 'YYMMDD');
	endDate := to_date(EDate,  'YYMMDD');
	OPEN Result FOR
		SELECT Eng_ID, Name, COUNT(Task_ID)
		FROM Engineers
		NATURAL JOIN Installations
		WHERE Install_date >= startDate AND Install_date <= endDate
		GROUP BY Eng_ID, Name
		UNION
		SELECT Eng_ID, Name, 0
		FROM Engineers
		WHERE Eng_ID NOT IN (SELECT Eng_ID
						  FROM Engineers
						  NATURAL JOIN Installations
						  WHERE Install_date >= startDate AND Install_date <= endDate);
END;
/

CREATE OR REPLACE PROCEDURE InstTaskDistriStat 
	(SDate IN char, EDate IN char, Result OUT sys_refcursor) AS
startDate	DATE;
endDate	DATE;
BEGIN
	startDate := to_date(SDate, 'YYMMDD');
	endDate := to_date(EDate, 'YYMMDD');
	OPEN Result FOR
		SELECT 'HK', (SELECT COUNT(Task_ID)
			FROM Subscriptions
			NATURAL JOIN Installations
			WHERE District = 'HK' AND (Install_date >= startDate AND Install_date <= endDate)) FROM DUAL
		UNION ALL
		SELECT 'KLN', (SELECT COUNT(Task_ID)
			FROM Subscriptions
			NATURAL JOIN Installations
			WHERE District = 'KLN' AND (Install_date >= startDate AND Install_date <= endDate)) FROM DUAL
		UNION ALL
		SELECT 'NT', (SELECT COUNT(Task_ID)
			FROM Subscriptions
			NATURAL JOIN Installations
			WHERE District = 'NT' AND (Install_date >= startDate AND Install_date <= endDate)) FROM DUAL;
END;
/
