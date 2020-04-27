package m19.core.rules;

import m19.core.Library;
import m19.core.user.User;
import m19.core.work.Work;

import java.io.Serializable; 

public class CheckPrice implements Rule, Serializable {

	private static CheckPrice _mainObject = new CheckPrice();
	private static final long serialVersionUID = 201901101348L;
	
	private CheckPrice(){}

	public static CheckPrice getInstance(){
		return _mainObject;
	}

	public int checkRule(User user, Work work){
		int price = work.getPrice();
		String userClassification = user.getBehaviorDescription();
		if (userClassification.equals("CUMPRIDOR")) return 0;
		else{
			if (price > 25) return 6;
			else return 0;
		}
	}

	public void nextRule(Library lib){
		lib.setRule(null);
	}
}