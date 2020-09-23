package m19.core.rules;

import m19.core.Library;
import m19.core.user.User;
import m19.core.work.Work;
import m19.core.Request;

import java.util.List;
import java.io.Serializable; 


public class CheckRequestTwice implements Rule, Serializable {
	
	private static CheckRequestTwice _mainObject = new CheckRequestTwice();
	private static final long serialVersionUID = 201901101348L;
	
	private CheckRequestTwice(){}

	public static CheckRequestTwice getInstance(){
		return _mainObject;
	}

	public int checkRule(User user, Work work) {
		List<Request>requests = user.getRequests();
		for (Request iter: requests){
			if (iter.hasWork(work)){
				return 1;
			}
		}
		return 0;
	}

	public void nextRule(Library lib){
		lib.setRule(CheckUserIsSuspended.getInstance());
	}
}