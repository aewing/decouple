<?hh // partial 
use Decouple\ORM\Connector;
class ConnectorTest extends PHPUnit_Framework_TestCase {
  private ?Connector $connector;
  private string $ORMTestDir = 'Tests/ORM/data';
  public function testPdo() : void {
    if(!is_dir($this->ORMTestDir)) {
      if(!mkdir($this->ORMTestDir, true)) {
        throw new \Exception(sprintf("Invalid ORM test data directory: %s", $this->ORMTestDir));
      }
    }
    /** Reset test data */
    $this->connector = null;
    $filename = sprintf("%s/test.sqlite", $this->ORMTestDir);
    if(file_exists($filename)) {
      unlink($filename);
    }
    touch($filename);
    $connector = $this->_connector();
    $this->assertEquals($connector->connected(), true);
  }
  public function testTableCreate() : void {
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
      if(!is_array($error)) {
        $error = ['Unknown', ''];
      }
      throw new \Exception(sprintf("Connector error: [%s] [%s]", $error[0], $error[1]));
    }
    $this->assertEquals($res, true);
  }
  public function testRowCreate() : void {
    $connector = $this->_connector();
    $articles = $connector->table('articles');
    $article = $articles->insert([
      'title' => 'Test Article',
      'content' => 'This is a test article'
    ]);
    $this->assertNotEquals(null, $article);
    if(!is_null($article)) {
      $this->assertEquals(1, $article['id']);
      $this->assertEquals('Test Article', $article['title']);
      $this->assertEquals('draft', $article['status']);
    }

    $article_two = $articles->insert([
      'title' => 'Test Article 2',
      'content' => 'This is another test article',
      'status' => 'pending'
    ]);
    $this->assertNotEquals(null, $article_two);
    if(!is_null($article_two)) {
      $this->assertEquals(2, $article_two['id']);
      $this->assertEquals('Test Article 2', $article_two['title']);
      $this->assertEquals('pending', $article_two['status']);
    }
  }
  public function testSelectMultiple() : void {
    $connector = $this->_connector();
    $articles = $connector->table('articles')->select()->fetchAll();
    $num_articles = $articles->count();
    $this->assertEquals(2, $num_articles);
  }
  public function testSelectColumn() : void {
    $connector = $this->_connector();
    $count = $connector->table('articles')->select(Vector {'COUNT(*)'})->fetchColumn();
    $this->assertEquals(2, $count);
  }
  public function testUpdateRow() : void {
    $connector = $this->_connector();
    $articles = $connector->table('articles');
    $updated = $articles->update(['title' => 'Test Article A'])->where('id', '=', 1)->execute();
    $this->assertEquals(0, $updated->errorCode());

    $article = $articles->select()->where('id','=',1)->fetch();
    $this->assertEquals('Test Article A', $article['title']);
  }
  public function testDeleteRow() : void {
    $connector = $this->_connector();
    $articles = $connector->table('articles');
    $deleted = $articles->delete()->where('id','=',1)->execute();
    $this->assertEquals(0, $deleted->errorCode());

    $count = $articles->select(Vector {'COUNT(*)'})->fetchColumn();
    $this->assertEquals(1, $count);
  }
  public function _connector() : Connector {
    if(is_null($this->connector)) {
      $filename = sprintf("%s/test.sqlite", $this->ORMTestDir);
      $this->connector = new Connector(sprintf("sqlite:%s", $filename), null, null);
    }
    return $this->connector;
  }
}
