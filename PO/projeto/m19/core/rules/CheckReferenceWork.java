package m19.core.rules;

import m19.core.Library;
import m19.core.user.User;
import m19.core.work.Work;
import m19.core.work.Category;

import java.io.Serializable; 

public class CheckReferenceWork implements Rule, Serializable {

	private static CheckReferenceWork _mainObejct = new CheckReferenceWork();
	private static final long serialVersionUID = 201901101348L;
	
	private CheckReferenceWork(){}

	public static CheckReferenceWork getInstance(){
		return _mainObejct;
	}

	public int checkRule(User user, Work work){
		if (work.getCategory() == Category.REFERENCE) return 5;
		else return 0;
	}

	public void nextRule(Library lib){
		lib.setRule(CheckPrice.getInstance());
	}
}