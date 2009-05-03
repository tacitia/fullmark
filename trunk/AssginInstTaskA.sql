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
		FindAvailEngD(charDate, R.District, availEngList);
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

