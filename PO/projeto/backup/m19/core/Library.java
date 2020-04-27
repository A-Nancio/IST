package m19.core;

import java.io.Serializable; 
import java.io.IOException;
import java.util.*;

import m19.core.exception.MissingFileAssociationException;
import m19.core.exception.BadEntrySpecificationException;
import m19.core.user.*;
import m19.core.work.*;

import m19.app.exception.NoSuchUserException;
import m19.app.exception.NoSuchWorkException;
/**
 * Class that represents the library as a whole.
 */
public class Library implements Serializable {

  /**
   * Serial number for serialization.
   */
  private static final long serialVersionUID = 201901101348L;
  private int _nextWorkId;
  private int _nextUserId;
  private Map<Integer, User> _users;
  private Map<Integer, Work> _works;
  private Date _time;

  public Library(){
    _nextUserId = 0;
    _nextWorkId = 0;
    _users = new HashMap<>();
    _works = new HashMap<>();
    _time = new Date();
  }

  public Date getTime(){
    return _time;
  }

  public int newUser(String userName, String email) {
    int id = _nextUserId;
    _nextUserId++;
    User newUser = new User(id, userName, email);
    _users.put(id, newUser);
    return id;
  }

  public void addBook(String title, String author, int price, Category category, int isbn, int numberOfCopies){
    int id = _nextWorkId;
    _nextWorkId++;
    Book newBook = new Book(id, title, author, price, category, isbn, numberOfCopies);
    _works.put(id, newBook);
  }

  public void addDVD(String title, String author, int price, Category category, int isbn, int numberOfCopies){
    int id = _nextWorkId;
    _nextWorkId++;
    DVD newDVD = new DVD(id, title, author, price, category, isbn, numberOfCopies);
    _works.put(id, newDVD);
  }

  public User getUser(int userId) throws NoSuchUserException{
    if (userId >= _nextUserId) throw new NoSuchUserException(userId);
    return  _users.get(userId);
  }

  protected List<User> getSortedUsers(){
    List<User> sortedUsers = new ArrayList<User>(_users.values());
    sortedUsers.sort(User.NameComparator);
    return sortedUsers;
  }

  public Work getWork(int workId) throws NoSuchWorkException{
    if (workId >= _nextWorkId || workId < 0) throw new NoSuchWorkException(workId);
    return _works.get(workId);
  }

  public List<Work> getSortedWorks(){
    return new ArrayList<Work>(_works.values());
  } 
}
