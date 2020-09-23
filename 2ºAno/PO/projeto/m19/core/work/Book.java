package m19.core.work;

public class Book extends Work {
    private String _author;
    private int _isbn;

    public Book(int id, String title, String author, int price, Category category, int isbn, int numberOfCopies){
        super(id, title, price, numberOfCopies, category);
        _author = author;
        _isbn = isbn;
    }

    @Override
    public String getDescription(){
        return super.getMainDescription() + " - Livro - " + 
        super.getTitle() + " - " + super.getPrice() +
        " - " + super.getCategory().toString() +
        " - " + _author + " - " + _isbn; 
    }

    @Override
    public boolean hasKeyWord(String keyWord){
        if (super.hasKeyWordInTitle(keyWord)) return true;
        else {
            return _author.toLowerCase().contains(keyWord);
        }
    }
}