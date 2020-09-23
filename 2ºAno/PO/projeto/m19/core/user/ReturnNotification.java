package m19.core.user;

public class ReturnNotification extends Notification {

	public ReturnNotification(String description){
		super(description);
	}

	public String getMessage(){
		return "ENTREGA: " + super.getDescription();
	}
}