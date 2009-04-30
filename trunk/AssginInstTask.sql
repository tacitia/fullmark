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