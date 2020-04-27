package m19.core.rules;

import m19.core.Library;
import m19.core.user.User;
import m19.core.work.Work;

import java.io.Serializable; 

public class CheckMaxRequisitions implements Rule, Serializable {

	private static CheckMaxRequisitions _mainObject = new CheckMaxRequisitions();
	private static final long serialVersionUID = 201901101348L;
	
	private CheckMaxRequisitions(){}

	public static CheckMaxRequisitions getInstance(){
		return _mainObject;
	}

	public int checkRule(User user, Work work) {
		if (user.hasMaxRequests()) return 4;
		else return 0;
	}

	public void nextRule(Library lib){
		lib.setRule(CheckReferenceWork.getInstance());
	}
}