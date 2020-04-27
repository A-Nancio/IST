package m19.core.work;

import java.util.*;
import java.io.Serializable;

public abstract class Work implements Serializable {
    private int _id;
    private int _price;
    private int _totalNumberOfCopies;
    private int _availableCopies;
    private String _title;
    private Category _category;
    private static final long _serialVersionUID = 201901101348L;

    public Work(int id, String title, int price, int numberOfCopies, Category cat){
        _id = id;
        _price = price;
        _totalNumberOfCopies = numberOfCopies;
        _availableCopies = _totalNumberOfCopies;
        _title = title;
        _category = cat;
    }

    public int getPrice(){
        return _price;
    }

    public int getId(){
        return _id;
    }

    public int getTotalNumberOfCopies() {
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
	
    public void requestWork(){

    }
}