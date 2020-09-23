package m19.core.rules;

import m19.core.Library;
import m19.core.user.User;
import m19.core.work.Work;

import java.io.Serializable; 

public class CheckUserIsSuspended implements Rule, Serializable {
	
	private static CheckUserIsSuspended _mainObject = new CheckUserIsSuspended();
	private static final long serialVersionUID = 201901101348L;
	private CheckUserIsSuspended(){}

	public static CheckUserIsSuspended getInstance(){
		return _mainObject;
	}

	public int checkRule(User user, Work work){
		if (!user.isActive()) return 2;
		else return 0;
	}
	
	public void nextRule(Library lib){
		lib.setRule(CheckNoCopiesLeft.getInstance());
	}
}