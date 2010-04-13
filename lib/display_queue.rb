class DisplayQueue
  include MongoMapper::Document

  key :str, String
  key :hejhopp, String
  key :published_at, Time
  timestamps!

  validates_length_of :str, :minimum => 1, :maximum => 140, :message => 'be nice say something'

  before_save :sanitize_str

  def sanitize_str
    self.str = self.str.gsub(/[^A-Za-z0-9]/,"_")
  end

  def to_s
    self.str
  end
end
