package m19.core.work;

import m19.core.Request;
import m19.core.NotificationManager;

import java.util.LinkedList;
import java.util.Iterator;
import java.io.Serializable;

public abstract class Work implements Serializable {
    private int _id;
    private int _price;
    private int _totalNumberOfCopies;
    private int _availableCopies;
    private String _title;
    private Category _category;
    private NotificationManager _manager;
    private LinkedList<Request> _requests = new LinkedList<>();
    private static final long _serialVersionUID = 201901101348L;

    public Work(int id, String title, int price, int numberOfCopies, Category cat){
        _id = id;
        _price = price;
        _totalNumberOfCopies = numberOfCopies;
        _availableCopies = numberOfCopies;
        _title = title;
        _category = cat;
        _manager = new NotificationManager();
    }

    public int getPrice(){
        return _price;
    }

    public int getId(){
        return _id;
    }

    public int getTotalCopies() {
        return _totalNumberOfCopies;
    }

    public int getAvailableCopies(){
        return _availableCopies;
    }

    public String getTitle() {
        return _title;
    }

    public Category getCategory(){
        return _category;
    }
    
    public boolean hasKeyWordInTitle(String keyWord){
        return _title.toLowerCase().contains(keyWord);
    }

    /**
   * Returns true if Work conatins keyWord String.
   * 
   * @param keyWord search term (in lower case)
   *
   */
    public abstract boolean hasKeyWord(String keyWord);
	
    public abstract String getDescription();

    public String getMainDescription(){
        return _id + " - " + _availableCopies + 
        " de " + _totalNumberOfCopies;
    }

    public NotificationManager getManager(){
        return _manager;
    }
    
    public void workBorrowed(Request req){
        _availableCopies--;
        _requests.addFirst(req);
    }


    public void workReturned(Request req){
        for (Iterator<Request> iterator = _requests.iterator(); iterator.hasNext();){
            Request temp = iterator.next();
            if (temp.equals(req)){
                iterator.remove();
                break;
            }
        }
        _availableCopies++;
    }
}