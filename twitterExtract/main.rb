require 'twitter'
require 'bindata'
client = Twitter::REST::Client.new do |config|

end

class MTwitter < BinData::Record
  uint32le :id_twitter
  string :usuario, :length => 20
  string :mensagem, :length => 280
  string :data, :length => 8
  string :pais, :length => 20
  string :hashtag, :length => 200
end


tweets = client.search('#Joker')


count = 0;

file = File.open("c:/temp/OrgArq/file.dat","wb")

tweets.each do |tweet|

  hashtag = "";
  tweet.hashtags.each do |p|
    hashtag =  hashtag + "#" +p.text.to_s
  end

  teste = MTwitter.new(:id_twitter => tweet.id, :usuario => tweet.user.name, :mensagem => tweet.full_text, :data => "09/09/09",:pais =>"sem pais", :hashtag => hashtag)


  file.write(teste.to_binary_s)



  puts count
  count = count + 1
end





