package m19.core.user;

import m19.core.Request;
import m19.core.Notification;

import java.util.HashSet;
import java.util.Set;
import java.util.Comparator;
import java.io.Serializable;

public class User implements Serializable, Observer {
	private int _id;
	private boolean _isActive;
	private String _name;
	private String _email;
	private UserBehavior _behavior;
	private Set<Request> _requests;
	private float _totalDebt;
	private static final long _serialVersionUID = 201901101348L;
	private boolean _isNotified;
	private List<Notification> _notifications;

	public User(int id, String name, String email) {
		_id = id;
		_isActive = true;
		_name = name;
		_email = email;
		_requests = new HashSet<>();
		_behavior = UserBehavior.NORMAL;
		_isNotified = false;
		_notifications = new LinkedList<Notification>();
	}

	protected boolean isActive() {
		return _isActive;
	}

	public String getName() {
		return _name;
	}

	@Override
	public String toString(){
		if (isActive()) {
			return _id + " - " + _name + " - " + _email + " - " + _behavior + " - ACTIVO";
		}
		else {
			return _id + " - " + _name + " - " + _email + _behavior + " - SUSPENSO - EUR " + _totalDebt;
		}
	}

	protected void setIsNotifed(boolean isNotifed) {
		_isNotified = isNotifed;
	}

	@Override
	protected void addNotification(Notification addNot) {
		if (!_isNotified) {
			for (Notification notification : _notifications) {
				if (addNot.getMessage().equals(notification.getMessage())) {
					return;
			}
			_notifications.add(addNot);
	}

	@Override
	protected String notify() {
		String s = new String();
		for (Notification notification : _notifications) {
			s = s + notification.getMessage() + "\n";
		}
		_notifications.clear();
		return s;
	}

	protected boolean isNotified() { return _isNotified; }

	protected void getNotifications() {
			return _notifications;
	}

	public static Comparator<User> NameComparator = new Comparator<User>() {
		public int compare(User user1, User user2){
			String userName1 = user1.getName().toUpperCase();
			String userName2 = user2.getName().toUpperCase();

			return userName1.compareTo(userName2);
		}
	};
}