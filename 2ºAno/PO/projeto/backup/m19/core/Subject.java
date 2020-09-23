package m19.core;

public interface Subject {
    void registerInterested(Observer observer);
    void unregisterInterested(Observer observer);
    void notifyAllInterested();
}