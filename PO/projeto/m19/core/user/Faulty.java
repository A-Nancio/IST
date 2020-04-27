package m19.core.user;

import java.io.Serializable; 

public class Faulty implements UserBehavior, Serializable {
	
	private static Faulty _instance;
	private static final long serialVersionUID = 201901101348L;

	private Faulty(){}

	public static Faulty getInstance(){
		if (_instance == null) _instance = new Faulty();
		return _instance;
	}

	public String getDescription(){
		return "FALTOSO";
	}

	public int getLowRequestTime(){
		return 2;
	}

	public int getMedRequestTime(){
		return 2;
	}

	public int getHighRequestTime(){
		return 2;	
	}

	public int getMaxRequests(){
		return 1;
	}

	public void checkBehavior(User user, int sucessDeliveries, int failDeliveries){
		if (sucessDeliveries >= 3) user.setBehavior(Normal.getInstance());
	}
}