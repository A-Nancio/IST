package m19.core.work;

public class Book extends Work {
    private String _author;
    private int _isbn;

    public Book(int id, String title, String author, int price, Category category, int isbn, int numberOfCopies){
        super(id, title, price, numberOfCopies, category);
        _author = author;
        _isbn = isbn;
    }

    public String getAuthor(){
        return _author;
    }

    public int getIsbn(){
        return _isbn;
    }

    public String toString(){
        return super.getId() + " - " + super.getTotalNumberOfCopies() + 
        " de " + super.getAvailableCopies() + " - Livro - " + 
        super.getTitle() + " - " + super.getPrice() + " - " + super.getCategory().toString() +
        " - " + _author + " - " + _isbn; 
    }
}