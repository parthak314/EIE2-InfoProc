from decimal import Decimal
from pprint import pprint
import boto3
from boto3.dynamodb.conditions import Key
from botocore.exceptions import ClientError

def delete_movie(title, year, dynamodb=None):
    if not dynamodb:
        dynamodb = boto3.resource('dynamodb', region_name='us-east-1')

    table = dynamodb.Table('Movies')

    try:
        response = table.delete_item(
            Key={
                'year': year,
                'title': title
            }
        )
    except ClientError as e:
        print(e.response['Error']['Message'])
    else:
        return response

def delete_pre_2000_movies(dynamodb=None):
    if not dynamodb:
        dynamodb = boto3.resource('dynamodb', region_name='us-east-1')

    table = dynamodb.Table('Movies')
    response = table.scan(FilterExpression=Key('year').lt(2000))
    movies = response['Items']

    for movie in movies:
        delete_movie(movie['title'], movie['year'], dynamodb)

    while 'LastEvaluatedKey' in response:
        response = table.scan(FilterExpression=Key('year').lt(2000), ExclusiveStartKey=response['LastEvaluatedKey'])
        movies = response['Items']
        for movie in movies:
            delete_movie(movie['title'], movie['year'], dynamodb)

if __name__ == '__main__':
    print("Deleting all movies released before 2000...")
    delete_pre_2000_movies()
    print("Deletion completed.")
