package m19.core;

import java.io.Serializable;
import m19.core.Request;

public class RequestNotification extends Notification implements Serializable {
    /*private static final long serialVersionUID = ... */

    public RequestNotification(String message) {
        super(message);
        _message = new String("A obra foi requisitada.");
    }

    public String getMessage() {
        return _message;
    }
}