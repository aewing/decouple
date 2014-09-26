<?hh // partial 
/**
 * Test the Decouple HTTP Request class
 */
class HTTPRequestTest extends PHPUnit_Framework_TestCase {
  /**
   * Test request
   */
  public function testRequest() : void {
    $uri = new Decouple\HTTP\Request\Uri('/foo/bar');
    $get = [
      'bar' => 'baz'
    ];
    $post = [
      'foo' => 'bar',
    ];
    $files = [];
    $request = new Decouple\HTTP\Request\Request($uri, Map::fromArray($get), Map::fromArray($post), Map::fromArray($files));

    $this->assertEquals(serialize($request->uri->parts()), serialize(Vector {'foo','bar'}));
    $this->assertEquals($request->getParam('bar'), 'baz');
    $this->assertEquals($request->postParam('foo'), 'bar');
  }
}
