package m19.core;

public interface Observer {
    void notify();
    void addNotification(Notification notification);
}