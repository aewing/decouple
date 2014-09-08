<?hh // strict
use HackPack\HackUnit\Core\TestCase;
/**
 * Test the Decouple HTTP Request class
 */
class HttpRequestTest extends TestCase {
  /**
   * Test request
   */
  public function testRequest() : void {
    $uri = new Decouple\Http\Request\Uri('/foo/bar');
    $get = [
      'bar' => 'baz'
    ];
    $post = [
      'foo' => 'bar',
    ];
    $files = [];
    $request = new Decouple\Http\Request\Request($uri, Map::fromArray($get), Map::fromArray($post), Map::fromArray($files));

    $this->expect(serialize($request->uri->parts()))->toEqual(serialize(Vector {'foo','bar'}));
    $this->expect($request->getParam('bar'))->toEqual('baz');
    $this->expect($request->postParam('foo'))->toEqual('bar');
  }
}
