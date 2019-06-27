var myApp = angular.module('myApp', []);

myApp.config(function() {
    // on app preparation, before run
});

myApp.run(function() {
    // on run
});

myApp.controller('AppController', ['$scope' /* dependency */, function($scope) {
    $scope.message = "Hello, world!";
    $scope.people = [
        {
            name: "Yoshi",
            age: "9",
            height: 72,
        },
        {
            name: "Jack",
            age: "19",
            height: 70,
        },
        {
            name: "Man",
            age: "91",
            height: 65,
        },
    ];
}]);