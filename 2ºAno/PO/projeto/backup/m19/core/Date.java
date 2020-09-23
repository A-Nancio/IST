package m19.core;

import java.io.Serializable;

public class Date implements Serializable {
	private int _currentDate;
	private static final long _serialVersionUID = 201901101348L;

	protected Date(){
		_currentDate = 0;
	}

	protected void advanceDay(int nDays){
		if (nDays >= 0) _currentDate += nDays;
	}

	public int getCurrentDate() {
		return _currentDate;
	}
}