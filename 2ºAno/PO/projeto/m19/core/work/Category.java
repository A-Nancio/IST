package m19.core.work;

public enum Category {
    REFERENCE("Referência"),
    FICTION("Ficção"),
    SCITECH("Técnica e Científica");

    private String _label;

    Category(String label){
        _label = label;
    }

    public String toString(){
        return _label;
    }
}