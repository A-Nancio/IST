package m19.core.user;

import java.io.Serializable; 
import m19.core.Request;

public class Normal implements UserBehavior, Serializable {
	
	private static Normal _instance;
	private static final long serialVersionUID = 201901101348L;

	private Normal(){}

	public static Normal getInstance(){
		if (_instance == null) _instance = new Normal();
		return _instance;
	}

	public String getDescription(){
		return "NORMAL";
	}

	public int getLowRequestTime(){
		return 3;
	}

	public int getMedRequestTime(){
		return 8;
	}

	public int getHighRequestTime(){
		return 15;	
	}

	public int getMaxRequests(){
		return 3;
	}

	public void checkBehavior(User user, int sucessDeliveries, int failDeliveries){
		if (sucessDeliveries >= 5) user.setBehavior(Abiding.getInstance());
		if (failDeliveries >= 3) user.setBehavior(Faulty.getInstance()); 
	}
}