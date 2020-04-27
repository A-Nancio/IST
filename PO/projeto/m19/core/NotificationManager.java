package m19.core;

import java.io.Serializable;
import java.util.List;
import java.util.ArrayList;
import java.util.Iterator;

import m19.core.user.Notification;
import m19.core.user.User;

public class NotificationManager implements Serializable{

	private List<User> _users;
    private static final long serialVersionUID = 201901101348L;
	
	public NotificationManager(){
		_users = new ArrayList<>();
	}

	public void notify(Notification notfcObject){
		for (User user: _users)
			user.addNotification(notfcObject);
		_users.clear();
	}

	public void addObserver(User user){
		_users.add(user);
	}
}