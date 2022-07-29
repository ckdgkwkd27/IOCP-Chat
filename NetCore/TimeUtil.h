#pragma once

class TimeUtil {
public:
	static const string CurrentDateTime() {
		time_t     now = time(0); //���� �ð��� time_t Ÿ������ ����
		struct tm  tstruct;
		int errNum;
		char       buf[80];
		errNum = localtime_s(&tstruct, &now);
		strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct); // YYYY-MM-DD.HH:mm:ss ������ ��Ʈ��

		return buf;
	}
};

