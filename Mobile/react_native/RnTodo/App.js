import { createAppContainer } from 'react-navigation';
import { createStackNavigator } from 'react-navigation-stack';

import { Primary } from './app/views/primary';
import { Dashboard } from './app/views/dashboard';
import { Settings } from './app/views/settings';

const AppNavigator = createStackNavigator(
  {
    primary: { screen: Primary },
    dashboard: { screen: Dashboard },
    settings: { screen: Settings, }
  },
  {
    defaultNavigationOptions: {
      headerShown: false
    },
    initialRouteName: "primary"
  }
);

export default createAppContainer(AppNavigator);