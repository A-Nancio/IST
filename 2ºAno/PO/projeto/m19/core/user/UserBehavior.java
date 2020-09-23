package m19.core.user;

public interface UserBehavior {	
	
	public String getDescription();
	
	public int getLowRequestTime();
	
	public int getMedRequestTime();
	
	public int getHighRequestTime();
	
	public int getMaxRequests();

	public void checkBehavior(User user, int sucessDeliveries, int failDeliveries);

}