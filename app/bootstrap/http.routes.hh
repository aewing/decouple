<?hh // partial 
// Load a map of routes
$routes = Map {
    "/" => "FrontController@index",
};
$app->addRoutes($routes);
// Add a single route
$app->addRoute("/bang/([^/]+)", function(Ui $ui, Log $log) {
  $log->write("Bang!");
  return "Bang!";
});
