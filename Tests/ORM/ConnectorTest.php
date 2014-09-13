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
    $this->assertEquals($res, true);
  }
  public function testRowCreate() {
    $connector = $this->_connector();
    $article = $connector->insert('articles', [
      'title' => 'Test Article',
      'content' => 'This is a test article'
    ]);
    $this->assertEquals(1, $article->id);
    $this->assertEquals('Test Article', $article->title);
    $this->assertEquals('pending', $article->status);
  }
  public function _connector() : Connector {
    if(is_null($this->connector)) {
      $filename = sprintf("%s/test.sqlite", ORMTestDir);
      $this->connector = new Connector(sprintf("sqlite:%s", $filename), null, null);
    }
    return $this->connector;
  }
}
