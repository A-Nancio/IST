package m19.core.work;

public class DVD extends Work {
    private String _director;
    private int _igac;

    public DVD(int id, String title, String director, int price, Category category, int igac, int numberOfCopies){
        super(id, title, price, numberOfCopies, category);
        _director = director;
        _igac = igac;
    }

    @Override
    public String getDescription(){
    	return super.getMainDescription() + " - DVD - " + 
        super.getTitle() + " - " + super.getPrice() + " - " +
        super.getCategory().toString() + " - " + _director + " - " + _igac;
    }

    @Override
    public boolean hasKeyWord(String keyWord){
        if (super.hasKeyWordInTitle(keyWord)) return true;
        else {
            return _director.toLowerCase().contains(keyWord);
        }
    }
}