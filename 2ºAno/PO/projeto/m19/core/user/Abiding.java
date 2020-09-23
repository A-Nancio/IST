package m19.core.user;

import java.io.Serializable; 

public class Abiding implements UserBehavior, Serializable {
	private static Abiding _instance;
	private static final long serialVersionUID = 201901101348L;

	private Abiding(){}

	public static Abiding getInstance(){
		if (_instance == null) _instance = new Abiding();
		return _instance;
	}

	public String getDescription(){
		return "CUMPRIDOR";
	}

	public int getLowRequestTime(){
		return 8;
	}

	public int getMedRequestTime(){
		return 15;
	}

	public int getHighRequestTime(){
		return 30;	
	}

	public int getMaxRequests(){
		return 5;
	}

	public void checkBehavior(User user, int sucessDeliveries, int failDeliveries){
		if (failDeliveries >= 1) user.setBehavior(Normal.getInstance());
	}
}