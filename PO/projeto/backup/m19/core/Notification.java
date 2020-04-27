package m19.core;

import java.io.Serializable;

public abstract class Notification implements Serializable {
    /*private static final long serialVersionUID = ... */
    private String _message;

    /*public Notification(String message) {
        _message = message;
    }*/

    protected String getMessage() {
        return _message;
    }
}