# CloudFormationについて

用語メモ

- Amazon VPC(Virtual Personal Cloud)
  - クラウド上に仮想的なネットワークを構築するためのサービスです。

- ALB (Application Load Balancer)
- NLB (Network Load Balancer)
- CLB (Classic Load Balancer)

## テンプレート

```yaml
AWSTemplateFormatVersion: 2010-09-09
Description: XXXXX

```

## VPC

```yaml
myVPC:
    Type: AWS::EC2::VPC
    Properties:
      CidrBlock: 10.0.0.0/16 
      EnableDnsSupport: true
      Tags:
        - Key: Name 
          Value: vpc
```

## subnet

```yaml
PublicSubnet:
    Type: AWS::EC2::Subnet
    Properties:
      AvailabilityZone: ap-northeast-1a 
      VpcId: !Ref myVPC 
      CidrBlock: 10.0.0.0/24 
      Tags:
        - Key: Name 
          Value: public-subnet 
```
