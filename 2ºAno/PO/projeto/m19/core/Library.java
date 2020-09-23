package m19.core;

import java.io.Serializable; 
import java.io.IOException;
import java.util.*;

import m19.core.user.*;
import m19.core.work.*;
import m19.core.rules.*;

import m19.app.exception.NoSuchUserException;
import m19.app.exception.NoSuchWorkException;
import m19.app.exception.RuleFailedException;
import m19.app.exception.WorkNotBorrowedByUserException;
import m19.app.exception.UserIsActiveException;
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
  private LinkedList<Request> _requests;
  private Date _time;
  private Rule _rule;

  public Library(){
    _users = new HashMap<>();
    _works = new HashMap<>();
    _time = new Date();
    _rule = CheckRequestTwice.getInstance();
    _requests = new LinkedList<>();
  }

  public int getTime(){
    return _time.getCurrentDate();
  }

  public void setRule(Rule rule){
    _rule = rule;
  }

  public void advanceDays(int nDays){
    _time.advanceDate(nDays);
    for (Request request: _requests){
      request.updateTime(nDays);
    }
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
    if (userId >= _nextUserId || userId < 0) throw new NoSuchUserException(userId);
    return  _users.get(userId);
  }

  public Work getWork(int workId) throws NoSuchWorkException{
    if (workId >= _nextWorkId || workId < 0) throw new NoSuchWorkException(workId);
    return _works.get(workId);
  }

  public String getUserDescription(int userId) throws NoSuchUserException{
    return getUser(userId).getDescription();
  }

  public String getWorkDescription(int workId) throws NoSuchWorkException{
    return getWork(workId).getDescription();
  }

  protected String getSortedUsers(){
    List<User> sortedUsers = new ArrayList<>(_users.values());
    sortedUsers.sort(User.NameComparator);
    String ret = "";
    for (User iter: sortedUsers){
      ret += iter.getDescription() + "\n";
    }
    return ret;
  }

  public String getSortedWorks(){
    List<Work> sortedWorks = new ArrayList<>(_works.values());
    String ret = "";
    for (Work iter: sortedWorks){
      ret += iter.getDescription() + "\n";
    }
    return ret;
  }

  public String keyWordSearch(String term){
    List<Work> works = new ArrayList<>(_works.values());
    String ret = "";
    for (Work iterator: works){
      if (iterator.hasKeyWord(term))
        ret += iterator.getDescription() + "\n";
    }
    return ret;
  }

  public String getNotifications(int userId) throws NoSuchUserException {
    User user = getUser(userId);
    return user.getNotifications();
  }
  
  public int returnWork(int userId, int workId) throws WorkNotBorrowedByUserException, NoSuchUserException, NoSuchWorkException {
    Work work = getWork(workId);
    User user = getUser(userId);
    Request request;
    int requestFine = 0;
    Rule checkWorkBorrowed = CheckRequestTwice.getInstance();
    
    if(checkWorkBorrowed.checkRule(user, work) == 0) throw new WorkNotBorrowedByUserException(workId, userId);
      Iterator<Request> iterator = _requests.iterator();
      while(iterator.hasNext()){
        request = iterator.next();
        if (request.hasWork(work) && request.hasUser(user)){
        requestFine = request.returnRequest();
        iterator.remove();
        break;
      }
    }

    return requestFine;
  }

  public int getUnpaidFine(int userId) throws NoSuchUserException{
    User user = getUser(userId);
    return user.getUnpaidFine();
  }

  public void payOffFine(int userId) throws NoSuchUserException, UserIsActiveException{
    User user = getUser(userId);
    if (user.isActive()) throw new UserIsActiveException(userId);
    user.payDebt();
  }

  public void addUnpaidFine(int userId, int fine) throws NoSuchUserException{
    User user = getUser(userId);
    user.applyUnpaidFine(fine);
  }

  public int makeRequest(int userId, int workId) throws RuleFailedException, NoSuchUserException, NoSuchWorkException{
    Work work = getWork(workId);
    User user = getUser(userId);
    
    while(_rule != null){
      int returnValue = _rule.checkRule(user, work);
      if (returnValue != 0) {
        _rule = CheckRequestTwice.getInstance();
        throw new RuleFailedException(userId, workId, returnValue);
      }
      _rule.nextRule(this);
    }
    
    Request newRequest = new Request(user, work);
    _requests.addFirst(newRequest);

    _rule = CheckRequestTwice.getInstance();
    return newRequest.getDeadline() + _time.getCurrentDate();
  }

  public void becomeNotified(int userId, int workId) throws NoSuchUserException, NoSuchWorkException{
    User user = getUser(userId);
    Work work = getWork(workId);

    NotificationManager manager = work.getManager();
    manager.addObserver(user);
  }
}
