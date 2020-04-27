package m19.core.rules;

import m19.core.Library;
import m19.core.user.User;
import m19.core.work.Work;

import java.io.Serializable; 

public class CheckNoCopiesLeft implements Rule, Serializable {
	
	private static CheckNoCopiesLeft _mainObject = new CheckNoCopiesLeft();
	private static final long serialVersionUID = 201901101348L;
	
	private CheckNoCopiesLeft(){}

	public static CheckNoCopiesLeft getInstance(){
		return _mainObject;
	}

	public int checkRule(User user, Work work) {
		int numCopies = work.getAvailableCopies();
		if (numCopies == 0) return 3;
		else return 0;
	}

	public void nextRule(Library lib){
		lib.setRule(CheckMaxRequisitions.getInstance());
	}
}