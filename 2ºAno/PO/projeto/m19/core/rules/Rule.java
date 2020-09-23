package m19.core.rules;

import m19.core.Library;
import m19.core.user.User;
import m19.core.work.Work;

public interface Rule {
	public int checkRule(User user, Work work);
	public void nextRule(Library lib);
}