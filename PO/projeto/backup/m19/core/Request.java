package m19.core;

import java.util.ArrayList;

public class Request implements Serialzable, Subject {
	private int _deadline;
	private ArrayList<Observer> _userList;

	public Request(int deadline){
		_deadline = deadline;
		_userList = new ArrayList<>();
	}

	public void reduceDeadline(int nDays){
		_deadline -= nDays;
	}

	@Override
	public void registerInterested(Observer observer) {
		if (observer == null) throw new NullPointerException;
		if (!_userList.constains(observer)) {
			_userList.add(observer);
		}
	}

	@Override
	public void unregisterInterested(Observer observer) {
		if (_userList.contains(observer)) {
			_userList.remove(user);
		}
	}

	@Override
	public void notifyAllInterested() {
		for (Observer observer : _userList) {
			user.addNotification(notification);
		}
	}
}