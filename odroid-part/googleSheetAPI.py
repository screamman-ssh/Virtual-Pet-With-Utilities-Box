from __future__ import print_function

import os.path

from google.auth.transport.requests import Request
from google.oauth2.credentials import Credentials
from google_auth_oauthlib.flow import InstalledAppFlow
from googleapiclient.discovery import build
from googleapiclient.errors import HttpError

# If modifying these scopes, delete the file token.json.
SCOPES = ['https://www.googleapis.com/auth/spreadsheets']

# The ID and range of a sample spreadsheet.
SPREADSHEET_ID = '1ZCuj0-3AKcwWawn5OAm4ufcgzkuO6D-n8VyA_kbY0GU'
SAMPLE_RANGE_NAME = 'Sheet1!A2:B'

def get_creds():
    creds = None
    if os.path.exists('token.json'):
        creds = Credentials.from_authorized_user_file('token.json', SCOPES)
    # If there are no (valid) credentials available, let the user log in.
    if not creds or not creds.valid:
        if creds and creds.expired and creds.refresh_token:
            creds.refresh(Request())
        else:
            flow = InstalledAppFlow.from_client_secrets_file(
                'credentials.json', SCOPES)
            creds = flow.run_local_server(port=0)
        # Save the credentials for the next run
        with open('token.json', 'w') as token:
            token.write(creds.to_json())
    return creds

def read_sheet():
    creds = get_creds()
    try:
        service = build('sheets', 'v4', credentials=creds)
        sheet = service.spreadsheets()
        result = sheet.values().get(spreadsheetId=SPREADSHEET_ID,range=SAMPLE_RANGE_NAME).execute()
        values = result.get('values', [])

        if not values:
            print('No data found.')
            return

        # print('Parameter, Value:')
        # for row in values:
        #     print('%s, %s' % (row[0], row[1]))
        return values
    except HttpError as err:
        print(err)

# def update_value(parameter, value):
#     creds = get_creds()
#     columns_range = {'Energy' : 'Sheet1!B2', 'Loveliness' :  'Sheet1!B3', 'Happiness' :  'Sheet1!B4', 'Feed' : 'Sheet1!B6', 'Pet' :  'Sheet1!B7'}
#     try:
#         service = build('sheets', 'v4', credentials=creds)
#         values = [
#             [value],
#         ]
#         body = {
#             'values': values
#         }
#         result = service.spreadsheets().values().update(spreadsheetId=SPREADSHEET_ID, range=columns_range[parameter],valueInputOption="USER_ENTERED", body=body).execute()
#         print(f"{result.get('updatedCells')} cells updated.")
#         return result
#     except HttpError as error:
#         print(f"An error occurred: {error}")
#         return error

def update_batches_value(values):
    creds = get_creds()
    columns_range = {'Energy' : 'Sheet1!B2', 'Loveliness' :  'Sheet1!B3', 'Happiness' :  'Sheet1!B4', 'Feed' : 'Sheet1!B6', 'Pet' :  'Sheet1!B7'}
    try:
        service = build('sheets', 'v4', credentials=creds)
        data = [
            {
                'range':  columns_range['Energy'],
                'values': [[values[0]]]
            },
            {
                'range':  columns_range['Loveliness'],
                'values': [[values[1]]]
            },
            {
                'range':  columns_range['Happiness'],
                'values': [[values[2]]]
            },
            {
                'range':  columns_range['Feed'],
                'values': [[values[3]]]
            },
            {
                'range':  columns_range['Pet'],
                'values': [[values[4]]]
            }
        ]
        body = {
            'valueInputOption' : "USER_ENTERED",
            'data' : data
        }
        result = service.spreadsheets().values().batchUpdate(spreadsheetId=SPREADSHEET_ID, body=body).execute()
        print(f"{result.get('updatedCells')} cells updated.")
        return result
    except HttpError as error:
        print(f"An error occurred: {error}")
        return error


if __name__ == '__main__':
    # update_value('Loveliness', 72)
    # update_batches_value()
    # read_sheet()
    pass
