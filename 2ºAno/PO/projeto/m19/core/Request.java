package m19.core;

import m19.core.work.Work;
import m19.core.user.*;

import java.io.Serializable;

public class Request implements Serializable {
	private int _deadLine;
	private User _userWhoRequested;
	private Work _requestedWork;
	private static final long serialVersionUID = 201901101348L;

	public Request(User user, Work work){
		_deadLine = user.detReqTime(work.getTotalCopies());
		_userWhoRequested = user;
		_requestedWork = work;
		work.workBorrowed(this);
		user.addRequest(this);
		NotificationManager notifier = work.getManager();
		//notifier.notify(new RequestNotification(work.getDescription()));
	}

	public int returnRequest(){
		_userWhoRequested.removeRequest(this);
		_requestedWork.workReturned(this);
		NotificationManager notifier = _requestedWork.getManager();
		notifier.notify(new ReturnNotification(_requestedWork.getDescription()));
		if (_deadLine >= 0) return 0;
		else return -(_deadLine * 5);
	}

	public User getUser(){
		return _userWhoRequested;
	}

	public Work getWork(){
		return _requestedWork;
	}

	public int getDeadline(){
		return _deadLine;
	}

	public boolean hasWork(Work work){
		return work.equals(_requestedWork);
	}

	public boolean hasUser(User user){
		return user.equals(_userWhoRequested);
	}

	public int reduceDeadline(int nDays){
		_deadLine -= nDays;
		return _deadLine;
	}

	public void updateTime(int nDays){
		int deadLine = reduceDeadline(nDays);
		if (deadLine < 0) {
			_userWhoRequested.suspendUser();
		}
	}

	@Override
	public boolean equals(Object o){
		if (o == this)
			return true;
		if (!(o instanceof Request)) return false;
		Request r = (Request) o;
		return _userWhoRequested.equals(r.getUser()) &&
		_requestedWork.equals(r.getWork());
	}
}