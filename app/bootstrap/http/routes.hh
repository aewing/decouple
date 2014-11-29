<?hh // partial 
use Decouple\Ui\Ui;
use Decouple\Log\Log;
// Load a map of routes
$routes = Map {
  "/" => "FrontController@index",
  "/test" => "FrontController@test",
};
$app->addRoutes($routes);
// Add a single route
$app->addRoute("/version", function(Ui $ui, Log $log, DebugRegistry $debug) {
  return 
    <layouts:front>
      <h1>Version</h1>
      <p class="segment">Decouple v0.1a</p>
      <p class="segment">HHVM v{HHVM_VERSION}</p>
      {perf_info($debug)}
    </layouts:front>
  ;
});
function perf_info(DebugRegistry $debug) : :xhp {
  $start = $debug->get('start_time');
  $end = getrusage();
  $comptime = rutime($end, $start, "utime");
  $calltime = rutime($end, $start, "stime");
  return 
    <div>
      <p class="ui segment">Computation time: {$comptime}ms</p>
      <p class="ui segment">Call time: {$calltime}ms</p>
    </div>
  ;
}
function rutime($ru, $rus, $index) {
      return ($ru["ru_$index.tv_sec"]*1000 + intval($ru["ru_$index.tv_usec"]/1000))
             -  ($rus["ru_$index.tv_sec"]*1000 + intval($rus["ru_$index.tv_usec"]/1000));
}
