package m19.core.user;

import java.io.Serializable;

public abstract class Notification implements Serializable {
    private static final long serialVersionUID = 201901101348L;
    private String _description;
    
    public Notification(String description) {
        _description = description;
    }

    public String getDescription(){
    	return _description;
    }
    
    public abstract String getMessage();
}