import pandas as pd
from sklearn.model_selection import train_test_split
from sklearn.preprocessing import StandardScaler
from sklearn.ensemble import RandomForestClassifier
from sklearn.metrics import classification_report, accuracy_score
import joblib

# Load the dataset
file_path = 'Bedroom_Fan_1.csv'  # Update with your file path if different
data = pd.read_csv(file_path)

# Select relevant columns
features = data[['temperature', 'humidity']]
target = data['speed']

# Split the data into training and testing sets
X_train, X_test, y_train, y_test = train_test_split(features, target, test_size=0.2, random_state=42)

# Standardize the feature data
scaler = StandardScaler()
X_train_scaled = scaler.fit_transform(X_train)
X_test_scaled = scaler.transform(X_test)

# Train the Random Forest classifier
classifier = RandomForestClassifier(random_state=42)
classifier.fit(X_train_scaled, y_train)

# Predict on the test data
y_pred = classifier.predict(X_test_scaled)

# Evaluate the model's performance
accuracy = accuracy_score(y_test, y_pred)
report = classification_report(y_test, y_pred)

# Print evaluation metrics
print(f'Accuracy: {accuracy}')
print(f'Classification Report:\n{report}')

# Save the model and scaler
joblib.dump(classifier, 'fan_speed_model.pkl')
joblib.dump(scaler, 'scaler.pkl')

# Code for loading and making predictions with the model
def predict_fan_speed(temperature, humidity):
    # Load the model and scaler
    classifier = joblib.load('fan_speed_model.pkl')
    scaler = joblib.load('scaler.pkl')
    
    # Prepare the data for prediction
    input_data = pd.DataFrame([[temperature, humidity]], columns=['temperature', 'humidity'])
    input_data_scaled = scaler.transform(input_data)
    
    # Make prediction
    prediction = classifier.predict(input_data_scaled)
    return prediction[0]

# Example usage
temperature = 30
humidity = 50
predicted_speed = predict_fan_speed(temperature, humidity)
print(f'Predicted Fan Speed: {predicted_speed}')