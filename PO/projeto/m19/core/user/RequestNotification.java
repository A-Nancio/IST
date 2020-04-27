package m19.core.user;

public class RequestNotification extends Notification {

	public RequestNotification(String message){
		super(message);
	}

	public String getMessage(){
		return "REQUISIÇÃO: " + super.getDescription();
	}
}