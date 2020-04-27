package m19.core.user;

import java.util.*;
import m19.core.Request;
import java.io.Serializable;

public class User implements Serializable{
	private static final long _serialVersionUID = 201901101348L;
	private static final int _debtPerDay = 5;
	private int _id;
	private int _successfulDeliveries;
	private int _failedDeliveries;
	private int _fine;

	private boolean _active;
	private String _name;
	private String _email;
	private UserBehavior _behavior;
	
	private LinkedList<Request> _requests;
	private List<Notification> _notifications;

	public User(int id, String name, String email) {
		_id = id;
		_active = true;
		_name = name;
		_email = email;
		_requests = new LinkedList<>();
		_notifications = new ArrayList<>();
		_behavior = Normal.getInstance();
	}

	public String getName() {
		return _name;
	}
	
	public boolean isActive() {
		return _active;
	}
	
	public void suspendUser(){
		_active = false;
	}

	public String getBehaviorDescription(){
		return _behavior.getDescription();
	}

	public void setBehavior(UserBehavior newBehavior){
		_behavior = newBehavior;
	}

	public String getDescription(){
		if (isActive()){
			return _id + " - " + _name + " - " + _email + " - " + _behavior.getDescription() + " - ACTIVO";
		}
		else{
			return _id + " - " + _name + " - " + _email + " - " + _behavior.getDescription() + " - SUSPENSO - EUR " + _fine;
		}
	}

	public int getUnpaidFine(){
		return _fine;
	}

	public void applyUnpaidFine(int amount){
		_fine += amount;
	}

	public void payDebt(){
		_fine = 0;
		for (Request iter: _requests){
			if (iter.getDeadline() < 0) return;
		}
		_active = true;
	}

	public List<Request> getRequests(){
		return new ArrayList<Request>(_requests);
	}

	public boolean hasMaxRequests(){
		return _requests.size() == _behavior.getMaxRequests();
	}

	public boolean hasActiveRequest(Request req){
		System.out.println(_requests.contains(req));
		return _requests.contains(req);
	}

	public void addRequest(Request req){
		_requests.addFirst(req);
	}

	public void removeRequest(Request req){
		Iterator<Request> iterator = _requests.iterator();
		while(iterator.hasNext()){
			Request aux = iterator.next();
			if (aux.equals(req)){
				if(aux.getDeadline() < 0) {
					_failedDeliveries++;
					_successfulDeliveries = 0;
				}
				else{ 
					_successfulDeliveries++;
					_failedDeliveries = 0;
				}
				iterator.remove();
				_behavior.checkBehavior(this, _successfulDeliveries, _failedDeliveries);
				break;
			}
		}
	}

	public int detReqTime(int numCopies){
		if (numCopies == 1) return _behavior.getLowRequestTime();
		else if (numCopies <= 5) return _behavior.getMedRequestTime();
		else return _behavior.getHighRequestTime();
	}

	public void addNotification(Notification ntfcObject){
		_notifications.add(ntfcObject);
	}

	public String getNotifications(){
		String ret = "";
		for (Notification iter: _notifications){
			ret += iter.getMessage() + "\n";
		}
		_notifications.clear();
		return ret;
	}

	public static Comparator<User> NameComparator = new Comparator<User>() {
		public int compare(User user1, User user2){
			String userName1 = user1.getName().toUpperCase();
			String userName2 = user2.getName().toUpperCase();

			return userName1.compareTo(userName2);
		}
	};
}