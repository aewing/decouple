<?hh // strict 
use Decouple\ORM\Connector;
define('ORMTestDir', dirname(__FILE__) . '/data');
class ConnectorTest extends PHPUnit_Framework_TestCase {
  private ?Connector $connector;
  public function testPdo() {
    if(!is_dir(ORMTestDir)) {
      if(!mkdir(ORMTestDir, true)) {
        throw new \Exception(sprintf("Invalid ORM test data directory: %s", ORMTestDir));
      }
    }
    /** Reset test data */
    $this->connector = null;
    $filename = sprintf("%s/test.sqlite", ORMTestDir);
    if(file_exists($filename)) {
      unlink($filename);
    }
    touch($filename);
    $connector = $this->_connector();
    $this->assertEquals($connector->connected(), true);
  }
  public function testTableCreate() {
    $connector = $this->_connector();
    $table = $connector->table('articles')->create();
    $table->identifier('id');
    $table->string('title', 55);
    $table->text('content')->nullable()->default(null);
    $table->enum('status', ['draft','pending','published'])->default('draft');
    $table->timestamps();
    $res = $connector->execute($table->sql(), true);
    if(!$res) {
      $error = $connector->error();
      throw new \Exception(sprintf("Connector error: [%s] [%s]", $error[0], $error[1]));
    }
    $this->assertEquals($res, true);
  }
  public function testRowCreate() {
    $connector = $this->_connector();
    $article = $connector->insert('articles', [
      'title' => 'Test Article',
      'content' => 'This is a test article'
    ], true);
    $this->assertEquals(1, $article['id']);
    $this->assertEquals('Test Article', $article['title']);
    $this->assertEquals('draft', $article['status']);

    $article_two = $connector->insert('articles', [
      'title' => 'Test Article 2',
      'content' => 'This is another test article',
      'status' => 'pending'
    ], true);
    $this->assertEquals(2, $article_two['id']);
    $this->assertEquals('Test Article 2', $article_two['title']);
    $this->assertEquals('pending', $article_two['status']);
  }
  public function testSelectMultiple() {
    $connector = $this->_connector();
    $statement = $connector->table('articles')->select();
    $articles = $connector->fetchAll($statement);
    $num_articles = $articles->count();
    $this->assertEquals(2, $num_articles);
  }
  public function testSelectColumn() {
    $connector = $this->_connector();
    $statement = $connector->table('articles')->select(Vector {'COUNT(*)'});
    $count = $connector->fetchColumn($statement);
    $this->assertEquals(2, $count);
  }
  public function testUpdateRow() {
    $connector = $this->_connector();
    $articles = $connector->table('articles');
    $query = $articles->update(['title' => 'Test Article A'])->where('id', '=', 1);
    $updated = $connector->execute($query);
    $this->assertEquals(1, $updated);

    $statement = $articles->select()->where('id','=',1);
    $article = $connector->fetch($statement);
    $this->assertEquals('Test Article A', $article['title']);
  }
  public function testDeleteRow() {
    $connector = $this->_connector();
    $articles = $connector->table('articles');
    $query = $articles->delete()->where('id','=',1);
    $deleted = $connector->execute($query);
    $this->assertEquals(1, $deleted);

    $query = $articles->select(Vector {'COUNT(*)'});
    $count = $connector->fetchColumn($query);
    $this->assertEquals(1, $count);
  }
  public function _connector() : Connector {
    if(is_null($this->connector)) {
      $filename = sprintf("%s/test.sqlite", ORMTestDir);
      $this->connector = new Connector(sprintf("sqlite:%s", $filename), null, null);
    }
    return $this->connector;
  }
}
