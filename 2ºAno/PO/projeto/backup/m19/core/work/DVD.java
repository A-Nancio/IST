package m19.core.work;

public class DVD extends Work {
    private String _director;
    private int _igac;

    public DVD(int id, String title, String director, int price, Category category, int igac, int numberOfCopies){
        super(id, title, price, numberOfCopies, category);
        _director = director;
        _igac = igac;
    }

    public String toString(){
    	return super.getId() + " - " + super.getTotalNumberOfCopies() + 
        " de " + super.getAvailableCopies() + " - DVD - " + 
        super.getTitle() + " - " + super.getPrice() + " - " +
        super.getCategory().toString() + " - " + _director + " - " + _igac;
    }
}